//==-- llvm/CodeGen/TargetLoweringObjectFileImpl.h - Object Info -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements classes used to handle lowerings specific to common
// object file formats.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_TARGETLOWERINGOBJECTFILEIMPL_H
#define LLVM_CODEGEN_TARGETLOWERINGOBJECTFILEIMPL_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/SectionKind.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

namespace llvm {
  class MachineModuleInfo;
  class Mangler;
  class MCAsmInfo;
  class MCExpr;
  class MCSection;
  class MCSectionMachO;
  class MCSymbol;
  class MCContext;
  class GlobalValue;
  class TargetMachine;


class TargetLoweringObjectFileELF : public TargetLoweringObjectFile {
  bool UseInitArray;

public:
  virtual ~TargetLoweringObjectFileELF() {}

  virtual void emitPersonalityValue(MCStreamer &Streamer,
                                    const TargetMachine &TM,
                                    const MCSymbol *Sym) const;

  /// getSectionForConstant - Given a constant with the SectionKind, return a
  /// section that it should be placed in.
  virtual const MCSection *getSectionForConstant(SectionKind Kind) const;


  virtual const MCSection *
  getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                           Mangler &Mang, const TargetMachine &TM) const;

  virtual const MCSection *
  SelectSectionForGlobal(const GlobalValue *GV, SectionKind Kind,
                         Mangler &Mang, const TargetMachine &TM) const;

  /// getTTypeGlobalReference - Return an MCExpr to use for a reference to the
  /// specified type info global variable from exception handling information.
  virtual const MCExpr *
  getTTypeGlobalReference(const GlobalValue *GV, unsigned Encoding,
                          Mangler &Mang, const TargetMachine &TM,
                          MachineModuleInfo *MMI, MCStreamer &Streamer) const;

  // getCFIPersonalitySymbol - The symbol that gets passed to .cfi_personality.
  virtual MCSymbol *
  getCFIPersonalitySymbol(const GlobalValue *GV, Mangler &Mang,
                          const TargetMachine &TM,
                          MachineModuleInfo *MMI) const;

  void InitializeELF(bool UseInitArray_);
  virtual const MCSection *
  getStaticCtorSection(unsigned Priority = 65535) const;
  virtual const MCSection *
  getStaticDtorSection(unsigned Priority = 65535) const;
};



class TargetLoweringObjectFileMachO : public TargetLoweringObjectFile {
public:
  virtual ~TargetLoweringObjectFileMachO() {}

  /// getDepLibFromLinkerOpt - Extract the dependent library name from a linker
  /// option string. Returns StringRef() if the option does not specify a library.
  virtual StringRef getDepLibFromLinkerOpt(StringRef LinkerOption) const;

  /// emitModuleFlags - Emit the module flags that specify the garbage
  /// collection information.
  virtual void emitModuleFlags(MCStreamer &Streamer,
                               ArrayRef<Module::ModuleFlagEntry> ModuleFlags,
                               Mangler &Mang, const TargetMachine &TM) const;

  bool isSectionAtomizableBySymbols(const MCSection &Section) const
      LLVM_OVERRIDE;

  virtual const MCSection *
  SelectSectionForGlobal(const GlobalValue *GV, SectionKind Kind,
                         Mangler &Mang, const TargetMachine &TM) const;

  virtual const MCSection *
  getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                           Mangler &Mang, const TargetMachine &TM) const;

  virtual const MCSection *getSectionForConstant(SectionKind Kind) const;

  /// shouldEmitUsedDirectiveFor - This hook allows targets to selectively
  /// decide not to emit the UsedDirective for some symbols in llvm.used.
  /// FIXME: REMOVE this (rdar://7071300)
  virtual bool shouldEmitUsedDirectiveFor(const GlobalValue *GV,
                                          Mangler &Mang,
                                          const TargetMachine &TM) const;

  /// getTTypeGlobalReference - The mach-o version of this method
  /// defaults to returning a stub reference.
  virtual const MCExpr *
  getTTypeGlobalReference(const GlobalValue *GV, unsigned Encoding,
                          Mangler &Mang, const TargetMachine &TM,
                          MachineModuleInfo *MMI, MCStreamer &Streamer) const;

  // getCFIPersonalitySymbol - The symbol that gets passed to .cfi_personality.
  virtual MCSymbol *
  getCFIPersonalitySymbol(const GlobalValue *GV, Mangler &Mang,
                          const TargetMachine &TM,
                          MachineModuleInfo *MMI) const;
};



class TargetLoweringObjectFileCOFF : public TargetLoweringObjectFile {
public:
  virtual ~TargetLoweringObjectFileCOFF() {}

  virtual const MCSection *
  getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                           Mangler &Mang, const TargetMachine &TM) const;

  virtual const MCSection *
  SelectSectionForGlobal(const GlobalValue *GV, SectionKind Kind,
                         Mangler &Mang, const TargetMachine &TM) const;

  /// getDepLibFromLinkerOpt - Extract the dependent library name from a linker
  /// option string. Returns StringRef() if the option does not specify a library.
  virtual StringRef getDepLibFromLinkerOpt(StringRef LinkerOption) const;

  /// emitModuleFlags - Emit Obj-C garbage collection and linker options.  Only
  /// linker option emission is implemented for COFF.
  virtual void emitModuleFlags(MCStreamer &Streamer,
                               ArrayRef<Module::ModuleFlagEntry> ModuleFlags,
                               Mangler &Mang, const TargetMachine &TM) const;
};

} // end namespace llvm

#endif
